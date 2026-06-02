const sliders = document.querySelectorAll("input[type='range']");

const pendingValues = {};
const inFlight = {};
let syncPausedUntil = 0;

const syncDelayAfterWebMs = 500;

const audioKeys = {
    0: "mic",
    1: "treble",
    2: "bass",
    3: "volume"
};

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

            if (pendingValues[id] !== undefined)
                sendLatest(id);
        });
}

function updateFromArduino() {
    if (Date.now() < syncPausedUntil)
        return;

    fetch("/api/audio")
        .then(response => response.json())
        .then(data => {
            sliders.forEach(slider => {
                const id = slider.dataset.id;
                const key = audioKeys[id];

                if (key && data[key] !== undefined)
                    slider.value = data[key];
            });
        })
        .catch(error => console.error("Audio sync failed:", error));
}

setInterval(updateFromArduino, 150);