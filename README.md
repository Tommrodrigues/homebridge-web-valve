<p align="center">
  <a href="https://github.com/homebridge/homebridge"><img src="https://raw.githubusercontent.com/homebridge/branding/master/logos/homebridge-color-round-stylized.png" height="140"></a>
</p>

<span align="center">

# homebridge-web-valve

[![npm](https://img.shields.io/npm/v/homebridge-web-valve.svg)](https://www.npmjs.com/package/homebridge-web-valve) [![npm](https://img.shields.io/npm/dt/homebridge-web-valve.svg)](https://www.npmjs.com/package/homebridge-web-valve)

</span>

## Description

This [homebridge](https://github.com/nfarina/homebridge) plugin exposes a web-based valve to Apple's [HomeKit](http://www.apple.com/ios/home/). Using simple HTTP requests, the plugin allows you to turn on/off the valve.

Find script samples for the valve controller in the _examples_ folder.

## Installation

1. Install [homebridge](https://github.com/nfarina/homebridge#installation-details)
2. Install this plugin: `npm install -g homebridge-web-valve`
3. Update your `config.json` file

## Configuration

```json
"accessories": [
     {
       "accessory": "WebValve",
       "name": "Valve",
       "apiroute": "http://myurl.com"
     }
]
```

### Core
| Key | Description | Default |
| --- | --- | --- |
| `accessory` | Must be `WebValve` | N/A |
| `name` | Name to appear in the Home app | N/A |
| `apiroute` | Root URL of your device | N/A |

## Optional fields
| Key | Description | Default |
| --- | --- | --- |
| `valveType` | Icon to be shown in the Home app (`0`, `1`, `2`, `3`) | `0` |

### Additional options
| Key | Description | Default |
| --- | --- | --- |
| `pollInterval` | Time (in seconds) between device polls | `300` |
| `listener` | Whether to start a listener to get real-time changes from the device | `false` |
| `timeout` | Time (in milliseconds) until the accessory will be marked as _Not Responding_ if it is unreachable | `3000` |
| `port` | Port for your HTTP listener (if enabled) | `2000` |
| `http_method` | HTTP method used to communicate with the device | `GET` |
| `username` | Username if HTTP authentication is enabled | N/A |
| `password` | Password if HTTP authentication is enabled | N/A |
| `model` | Appears under the _Model_ field for the accessory | plugin |
| `serial` | Appears under the _Serial_ field for the accessory | apiroute |
| `manufacturer` | Appears under the _Manufacturer_ field for the accessory | author |
| `firmware` | Appears under the _Firmware_ field for the accessory | version |

## API Interfacing

Your API should be able to:

1. Return JSON information when it receives `/status`:
```
{
    "currentState": INT_VALUE
}
```

2. Set the state when it receives:
```
/setState?value=INT_VALUE
```

### Optional (if listener is enabled)

1. Update `state` following a manual override by messaging the listen server:
```
/state?value=INT_VALUE
```
