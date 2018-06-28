# png-decoder-javascript
An aggressively optimized png loader for javascript that decodes `png` images `8x` faster than alternatives. See benchmark:

## Benchmark 
Decoding a `4096x4096 RGB` image:
| Algorithm     | timing (s)    | 
| ------------- |:-------------:|
| **this**      | 0.5           |
| pngtoy        | 4.1           |
| HTML Image    | 1.5           |

## Compability
This can handle following kinds of `png`formats:
- `gray 8bit`
- `gray 16bit`
- `RGB`
- `RGBA` 

## Usage
See in `example.mjs` for a sample usage

## How-To Run with Node.js
- `node --experimental-modules --napi-modules example.mjs`

## How-TO Run in the Browser
- Just `import LoaderPNG from "./LoaderPNG";` 

