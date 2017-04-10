# 21days

Firmware designed for the Particle Photon.

To flash your Photon, download the desktop IDE[1] or use the online IDE[2].

[1] https://www.particle.io/products/development-tools/particle-local-ide
[2] https://build.particle.io/build

iOS setup:
- `brew install watchman`
- `brew install node`
- `npm install -g react-native-cli`
- `cd ios`
- add a file called token.js that contains `export const api_key = "HEROKU_KEY"; export const device_id = "PHOTON_ID";`
- `npm install`

For release on the phone build in ios:

- npm run build
- open xcode, click play

For debug build on iphone simulator:
- react-native run-ios
