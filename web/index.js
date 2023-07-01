const BLE_NAME = 'WirelessPWMFan'
const SERVICE_UUID = '7b742148-fe4e-411e-a762-fd5967d77491'
const CHAR_UUID = '10971da0-aa16-4042-a054-a1501bb3f2d5'

const MAX_PIN_COUNT = 2;

let char, server, charPromise;

let pinIndex = 0;

async function getChar() {
    if (charPromise) {
        return charPromise;
    }
    return charPromise = new Promise(async (res, rej) => {
        try {
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
            res(char);
            charPromise = undefined;
        } catch (e) {
            rej(e);
        }
    });
}

async function sendByBLE(percent) {
    const char = getChar();
    await char.writeValue(Uint8Array.of(1, Math.floor(255 * (percent / 100))))
    await char.writeValue(Uint8Array.of(2, pinIndex))
}

async function changePin() {
    if (pinIndex >= MAX_PIN_COUNT - 1) {
        pinIndex = 0;
    } else {
        pinIndex += 1;
    }
    await char.writeValue(Uint8Array.of(2, pinIndex))
}

const slider = new mdc.slider.MDCSlider(document.querySelector('.mdc-slider'))
slider.listen('MDCSlider:change', e => {
    const percent = e.detail.value
    document.querySelector('.current-value-wrapper').innerHTML = `${percent}%`
    sendByBLE(percent)
})
