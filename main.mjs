import LoaderPNG from "./LoaderPNG";
import fs from "fs";

function dummy() {
    return new Promise((resolve, reject) => {
		resolve(database);
	});
}


function xhr(type, url) {
    return new Promise((resolve, reject) => {
        let xhr0 = new XMLHttpRequest();
        xhr0.onreadystatechange = function() {
            if (this.readyState === 4 && this.status === 200) {
                resolve(this.response);
            }
        };
        xhr0.open(type, url, true);
        xhr0.send();
    });
};

function array2heap(arr, module) {
	let nDataBytes = arr.length*arr.BYTES_PER_ELEMENT
	let dataPtr = module._malloc(nDataBytes);

	let dataHeap = new Uint8Array(module.HEAPU8.buffer, dataPtr, nDataBytes);
	dataHeap.set(new Uint8Array(arr.buffer));

	return dataHeap;
}

function run() {

	fs.readFile('./test.png', function read(err, data) {
		if (err)
			throw err;

		let png = new LoaderPNG();

		let in_heap = array2heap(data, png)

		let out_ptr = png._malloc(10*1000*1000*3);
		let out_heap = new Uint8Array(png.HEAPU8.buffer, out_ptr, 10*1000*1000*3);

		png.load(in_heap.byteOffset, out_heap.byteOffset, 4, 4, 3);
		let result = new Uint8Array(out_heap.buffer, out_heap.byteOffset, out_heap.length);
		//console.log(result)
		
		png._free(in_heap.byteOffset);
		png._free(out_heap.byteOffset);

	});
}


run()
