let char, server;

async function sendByBLE(percent) {
    if (!char || !server.connected) {
        const device = await navigator.bluetooth.requestDevice(
            {
                filters:[
                    {
                        name:'WirelessPWMFan',
                    },
                ],
                optionalServices: ['7b742148-fe4e-411e-a762-fd5967d7749b'],
            },
        )
        server = await device.gatt.connect()
        const service = await server.getPrimaryService('7b742148-fe4e-411e-a762-fd5967d7749b')
        char = await service.getCharacteristic('10971da0-aa16-4042-a054-a1501bb3f2d5')
    }
    await char.writeValue(Uint8Array.of(1, Math.floor(255 * (percent / 100))))
}

const slider = new mdc.slider.MDCSlider(document.querySelector('.mdc-slider'));
slider.listen('MDCSlider:change', e => {
    const percent = e.detail.value
    document.querySelector('.current-value-wrapper').innerHTML = `${percent}%`
    sendByBLE(percent)
})
