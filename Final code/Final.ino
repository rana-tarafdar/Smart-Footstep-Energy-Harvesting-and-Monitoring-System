#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* ================= WIFI (ACCESS POINT) ================= */
const char *ap_ssid = "SFEHM_SYSTEM";
const char *ap_password = "12345678";

/* ================= WEB SERVER ================= */
WebServer server(80);

/* ================= CLINTS ================= */
int wifiClients = 0;

/* ================= OLED ================= */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* ================= PINS ================= */
#define PIEZO_PIN 34
#define LDR_PIN 32
#define LED_PIN 25

/* ================= CONSTANTS ================= */
#define ADC_MAX 4095.0
#define VREF 3.3

int stepThreshold = 1240;
int ldrThreshold = 2000;
unsigned long stepDelay = 250;

/* ================= VARIABLES ================= */
int steps = 0;
int lastADC = 0;
unsigned long lastStepTime = 0;

float dcVoltage = 0.0;
float estimatedVoltage = 0.0;
bool isDark = false;
bool ledState = false;

/* ================= MAX VOLTAGE TRACKING ================= */
float maxDC_AllTime = 0.0;
float maxAC_AllTime = 0.0;

// float maxDC_Web = 0.0;
// float maxAC_Web = 0.0;

// unsigned long lastWebReset = 0;
// const unsigned long webResetInterval = 2000; // 2 seconds

/* ================= WEB PAGE ================= */
String webpage()
{
    String html = R"rawliteral(
  
<!doctype html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <title>SFEHM Dashboard</title>
        <style>
            /* Nature-themed styles */
            body {
                font-family: 'Segoe UI', Arial, sans-serif;
                color: #f7fbf4;
                padding: 28px;
                min-height: 100vh;
                background:
                    linear-gradient(180deg, rgba(14,64,37,0.9) 0%, rgba(30,120,69,0.85) 60%);
                background-blend-mode: overlay;
            }

            h1 {
                text-align: center;
                margin-bottom: 6px;
                font-weight: 700;
                letter-spacing: 0.6px;
                color: #f2fff5;
            }

            p.lead {
                text-align: center;
                margin-top: 0;
                font-size: 0.8rem;
                opacity: 0.95;
                color: #e9f7ee;
            }

            p.footer {
                text-align: center;
                margin-top: 40px;
                font-size: 0.85rem;
                opacity: 0.9;
                color: #dff3e6;
            }

            .grid {
                display: grid;
                grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
                gap: 20px;
            }

            .card {
                background: linear-gradient(180deg, rgba(255,255,255,0.03), rgba(255,255,255,0.02));
                border: 1px solid rgba(34, 85, 48, 0.18);
                padding: 20px;
                border-radius: 14px;
                box-shadow: 0 8px 20px rgba(6, 36, 18, 0.45);
                transition: transform 250ms ease, box-shadow 250ms ease, border-color 250ms ease;
            }

            .card:hover {
                transform: translateY(-8px) scale(1.02);
                box-shadow: 0 18px 40px rgba(14, 70, 36, 0.55);
                border-color: rgba(56, 160, 100, 1);
            }

            .value {
                font-size: 2rem;
                font-weight: 700;
                display: inline-block;
                color: #fbfff9;
            }

            .small {
                opacity: 0.95;
                font-size: 0.95rem;
                margin-bottom: 8px;
                display: block;
                color: #dff3e6;
            }

            /* small leaf icon next to titles */
            .small::before {
                content: "\1F331";
                margin-right: 8px;
            }

            /* decorative vine under the title */
            .vine {
                display: block;
                width: 100%;
                max-width: 420px;
                margin: 6px auto 18px auto;
                opacity: 0.95;
            }
        </style>
    </head>
    <body>
        <h1>Smart Footstep Energy Harvesting</h1>
        <svg class="vine" viewBox="0 0 600 40" xmlns="http://www.w3.org/2000/svg" aria-hidden="true">
            <path d="M10 30 C150 -10, 450 -10, 590 30" stroke="#c5f1d0" stroke-opacity="0.9" stroke-width="4" fill="none" stroke-linecap="round"/>
            <g fill="#b8efc8" fill-opacity="0.95">
                <ellipse cx="60" cy="12" rx="8" ry="4" transform="rotate(-20 60 12)"/>
                <ellipse cx="180" cy="6" rx="6" ry="3" transform="rotate(-10 180 6)"/>
                <ellipse cx="360" cy="8" rx="7" ry="3.5" transform="rotate(10 360 8)"/>
                <ellipse cx="520" cy="12" rx="8" ry="4" transform="rotate(20 520 12)"/>
            </g>
        </svg>
        <p class="lead">
            Project created by <br />
            Rana, Abhradeep, Rajdeep, Akanshu, Sanchita
        </p>

        <div class="grid">
            <div class="card">
                <div class="small">WiFi Clients</div>
                <div class="value">
                    )rawliteral"; html += wifiClients; html += R"rawliteral(
                </div>
            </div>

            <div class="card">
                <div class="small">Footsteps</div>
                <div class="value">
                    )rawliteral"; html += steps; html += R"rawliteral(
                </div>
            </div>

            <div class="card">
                <div class="small">Time Status</div>
                <div class="value">
                    )rawliteral"; html += (isDark ? "Night" : "Day"); html +=
                    R"rawliteral(
                </div>
            </div>

            <div class="card">
                <div class="small">LED Status</div>
                <div class="value">
                    )rawliteral"; html += (ledState ? "ON" : "OFF"); html +=
                    R"rawliteral(
                </div>
            </div>

            <div class="card">
                <div class="small">Max DC (All Time)</div>
                <div class="value">
                    )rawliteral"; html += String(maxDC_AllTime, 2); html +=
                    R"rawliteral( V
                </div>
            </div>

            <div class="card">
                <div class="small">Max AC (All Time)</div>
                <div class="value">
                    )rawliteral"; html += String(maxAC_AllTime, 2); html +=
                    R"rawliteral( V
                </div>
            </div>
        </div>

        <p class="footer">ESP32 Local Network - Academic Demonstration</p>
        <br />

        <script>
            setTimeout(() => location.reload(), 2000);
        </script>
    </body>
</html>

)rawliteral";

    return html;
}

/* ================= SETUP ================= */
void setup()
{
    Serial.begin(115200);
    Wire.begin(21, 22);

    analogSetPinAttenuation(PIEZO_PIN, ADC_11db);
    analogSetPinAttenuation(LDR_PIN, ADC_11db);

    pinMode(LED_PIN, OUTPUT);

    display.begin(0x3C, true);
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);

    /* ---- START ACCESS POINT ---- */
    WiFi.softAP(ap_ssid, ap_password);
    IPAddress ip = WiFi.softAPIP();

    Serial.print("ESP32 AP IP: ");
    Serial.println(ip);

    server.on("/", []()
              { server.send(200, "text/html", webpage()); });
    server.begin();
}

/* ================= LOOP ================= */
void loop()
{
    server.handleClient();
    wifiClients = WiFi.softAPgetStationNum();

    /* ---- PIEZO READ ---- */
    int piezoADC = analogRead(PIEZO_PIN);
    dcVoltage = (piezoADC / ADC_MAX) * VREF;

    if (dcVoltage > 0)
    {
        estimatedVoltage = dcVoltage + 1.4;
    }
    else
    {
        estimatedVoltage = dcVoltage;
    }

    /* ---- STEP COUNT ---- */
    if (piezoADC > stepThreshold && lastADC <= stepThreshold)
    {
        if (millis() - lastStepTime > stepDelay)
        {
            steps++;
            lastStepTime = millis();
        }
    }
    lastADC = piezoADC;

    /* ---- MAX VOLTAGE UPDATE ---- */
    if (dcVoltage > maxDC_AllTime)
        maxDC_AllTime = dcVoltage; //             IMP LINE
    if (estimatedVoltage > maxAC_AllTime)
        maxAC_AllTime = estimatedVoltage; //             IMP LINE

    // --------------------------------------------------------------------

    // if (dcVoltage > maxDC_Web) maxDC_Web = dcVoltage;
    // if (estimatedVoltage > maxAC_Web) maxAC_Web = estimatedVoltage;

    // if (millis() - lastWebReset >= webResetInterval) {
    // maxDC_Web = 0.0;
    // maxAC_Web = 0.0;
    // lastWebReset = millis();
    // }

    // -------------------------------------------------------------------

    /* ---- LDR ---- */
    int ldrADC = analogRead(LDR_PIN);
    isDark = (ldrADC < ldrThreshold);
    ledState = isDark;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);

    /* ---- OLED DISPLAY ---- */
    display.clearDisplay();
    display.drawRect(0, 0, 128, 64, SH110X_WHITE);

    display.setCursor(10, 2);
    display.println("SFEHM System");

    display.setCursor(4, 14);
    display.print("DC: ");
    display.print(dcVoltage, 2);
    display.print(" V  | ");         // Made changes here
    display.print(maxDC_AllTime, 2); // Made changes here
    display.println(" V");

    display.setCursor(4, 26);
    display.print("AC: ~");
    display.print(estimatedVoltage, 2);
    display.print(" V | ");          // Made changes here
    display.print(maxAC_AllTime, 2); // Made changes here
    display.println(" V");

    display.setCursor(4, 38);
    display.print("Time: ");
    display.println(isDark ? "NIGHT" : "DAY");

    display.setCursor(4, 50);
    display.print("Light: ");
    display.println(isDark ? "On" : "Off");

    display.display();

    delay(50);
}