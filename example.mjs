import LoaderPNG from "./LoaderPNG";
import fs from "fs";

function array2heap(arr, module) {
	let nDataBytes = arr.length*arr.BYTES_PER_ELEMENT
	let dataPtr = module._malloc(nDataBytes);

	let dataHeap = new Uint8Array(module.HEAPU8.buffer, dataPtr, nDataBytes);
	dataHeap.set(new Uint8Array(arr.buffer));

	return dataHeap;
}

function run() {

	fs.readFile('./dog.png', function read(err, data) {
		if (err)
			throw err;

		let png = new LoaderPNG();

		let in_heap = array2heap(data, png)

		let out_ptr = png._malloc(640*480*3);
		let out_heap = new Uint8Array(png.HEAPU8.buffer, out_ptr, 640*480*3);

		png.load(in_heap.byteOffset, out_heap.byteOffset, 640, 480, 3);
		let result = new Uint8Array(out_heap.buffer, out_heap.byteOffset, out_heap.length);
		
		png._free(in_heap.byteOffset);
		png._free(out_heap.byteOffset);

	});
}


run()
