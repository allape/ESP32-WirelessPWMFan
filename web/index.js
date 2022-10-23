const BLE_NAME = 'WirelessPWMFan'
const SERVICE_UUID = '7b742148-fe4e-411e-a762-fd5967d77491'
const CHAR_UUID = '10971da0-aa16-4042-a054-a1501bb3f2d5'

let char, server;

async function sendByBLE(percent) {
    if (!char || !server.connected) {
        const device = await navigator.bluetooth.requestDevice(
            {
                filters:[
                    {
                        name: BLE_NAME,
                    },
                ],
                optionalServices: [SERVICE_UUID],
            },
        )
        server = await device.gatt.connect()
        const service = await server.getPrimaryService(SERVICE_UUID)
        char = await service.getCharacteristic(CHAR_UUID)
    }
    await char.writeValue(Uint8Array.of(1, Math.floor(255 * (percent / 100))))
}

const slider = new mdc.slider.MDCSlider(document.querySelector('.mdc-slider'));
slider.listen('MDCSlider:change', e => {
    const percent = e.detail.value
    document.querySelector('.current-value-wrapper').innerHTML = `${percent}%`
    sendByBLE(percent)
})
