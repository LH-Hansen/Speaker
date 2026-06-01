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