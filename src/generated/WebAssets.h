#pragma once

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>UNO R4 Sliders</title>
  <link rel="stylesheet" href="/style.css">
</head>
<body>
  <main class="eq-panel">

    <div class="channel">
    <input type="range" min="0" max="270" value="2" data-id="0">
    <span class="label">Mic</span>
</div>

<div class="channel">
    <input type="range" min="0" max="270" value="50" data-id="1">
    <span class="label">Treble</span>
</div>

<div class="channel">
    <input type="range" min="0" max="270" value="50" data-id="2">
    <span class="label">Base</span>
</div>

<div class="channel">
    <input type="range" min="0" max="270" value="50" data-id="3">
    <span class="label">Volume</span>
</div>

</main>

  <script src="/script.js"></script>
</body>
</html>
)rawliteral";

const char STYLE_CSS[] PROGMEM = R"rawliteral(
html,
body {
    height: 100%;
    margin: 0;
    overflow: hidden;
    background:
        radial-gradient(circle at center, #3a3a3a 0%, #151515 75%);
}

body {
    display: flex;
    justify-content: center;
    align-items: center;
}

.eq-panel {
    display: flex;
    gap: 64px;
    padding: 72px 96px;
    border-radius: 32px;

    background: #222;

    box-shadow:
        inset 0 1px 2px rgba(255,255,255,0.08),
        0 18px 40px rgba(0,0,0,0.55);
}

.channel {
    display: flex;
    flex-direction: column;
    align-items: center;
    gap: 24px;
}

.label {
    font-family: Arial, sans-serif;
    font-size: 18px;
    font-weight: bold;
    color: #c7c7c7;
    text-transform: uppercase;
    letter-spacing: 2px;
}

input[type="range"] {
    -webkit-appearance: none;
    appearance: none;

    width: 390px;
    height: 80px;

    transform: rotate(-90deg);

    margin: 155px -155px;

    background:
        repeating-linear-gradient(
            to right,
            #777 0,
            #777 2px,
            transparent 2px,
            transparent 36px
        )
        no-repeat center 62px / 360px 16px;

    cursor: pointer;
}

/* Track */

input[type="range"]::-webkit-slider-runnable-track {
    width: 390px;
    height: 12px;

    border-radius: 12px;

    background:
        linear-gradient(#555, #222) padding-box,
        linear-gradient(#999, #333) border-box;

    border: 2px solid transparent;

    box-shadow:
        inset 0 1px 2px rgba(255,255,255,0.25),
        0 1px 2px rgba(0,0,0,0.9);
}

/* Thumb */

input[type="range"]::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;

    width: 50px;
    height: 92px;

    margin-top: -40px;

    border-radius: 10px;
    border: 1px solid #777;

    background:
        linear-gradient(
            90deg,
            #f1f1f1 0%,
            #a8a8a8 50%,
            #4d4d4d 100%
        );

    box-shadow:
        -12px 0 20px rgba(0,0,0,0.75),
        inset 1px 0 1px rgba(255,255,255,0.8),
        inset -2px 0 2px rgba(0,0,0,0.45);

    cursor: ns-resize;
}

/* Firefox */

input[type="range"]::-moz-range-track {
    height: 12px;
    border-radius: 12px;
    background: #333;
}

input[type="range"]::-moz-range-thumb {
    width: 50px;
    height: 92px;

    border-radius: 10px;
    border: 1px solid #777;

    background:
        linear-gradient(
            90deg,
            #f1f1f1,
            #777
        );

    cursor: ns-resize;
}

input[type="range"]:focus {
    outline: none;
}
)rawliteral";

const char SCRIPT_JS[] PROGMEM = R"rawliteral(
const sliders = document.querySelectorAll("input[type='range']");

const pendingValues = {};
const inFlight = {};
let syncPausedUntil = 0;

const syncDelayAfterWebMs = 500;

sliders.forEach(slider => {
    slider.addEventListener("input", () => {
        const id = slider.dataset.id;
        const value = slider.value;

        syncPausedUntil = Date.now() + syncDelayAfterWebMs;

        pendingValues[id] = value;
        sendLatest(id);
    });
});

function sendLatest(id) {
    if (inFlight[id])
        return;

    const value = pendingValues[id];

    if (value === undefined)
        return;

    delete pendingValues[id];
    inFlight[id] = true;

    fetch(`/api/slider?id=${id}&value=${value}`)
        .catch(error => console.error("Send failed:", error))
        .finally(() => {
            inFlight[id] = false;

            if (pendingValues[id] !== undefined) {
                sendLatest(id);
            }
        });
}

function updateFromArduino() {
    if (Date.now() < syncPausedUntil)
        return;

    fetch("/api/audio")
        .then(response => response.json())
        .then(data => {
            sliders[0].value = data.mic;
            sliders[1].value = data.volume;
            sliders[2].value = data.treble;
            sliders[3].value = data.bass;
        })
        .catch(error => console.error("Audio sync failed:", error));
}

setInterval(updateFromArduino, 150);
)rawliteral";

