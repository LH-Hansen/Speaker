const sliders = document.querySelectorAll("input[type='range']");

const sendDelayMs = 40;
const timers = {};

sliders.forEach(slider => {
    slider.addEventListener("input", () => {
        const id = slider.dataset.id;
        const value = slider.value;

        clearTimeout(timers[id]);

        timers[id] = setTimeout(() => {
            fetch(`/api/slider?id=${id}&value=${value}`)
                .catch(error => console.error("Send failed:", error));

            console.log(`Slider ${id}: ${value}`);
        }, sendDelayMs);
    });
});