/*
Compare the SIMD wrappers in WebAssembly with and without the simd128 backend

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

import fs from "fs";

const rounds = Number(process.argv[2] || 1000);

function load(file) {
	return new WebAssembly.Instance(new WebAssembly.Module(fs.readFileSync(file))).exports;
}

function run(wasm) {
	const count = wasm.run();
	const offsets = new Int32Array(wasm.memory.buffer, wasm.offsets.value, count + 1).slice();
	const out = new Uint8Array(wasm.memory.buffer, wasm.out.value, offsets[count]).slice();
	return { count, offsets, out };
}

function name(wasm, i) {
	const names = new Uint8Array(wasm.memory.buffer, wasm.names.value);
	const begin = new Int32Array(wasm.memory.buffer, wasm.name_offsets.value, i + 1)[i];
	return new TextDecoder().decode(names.subarray(begin, names.indexOf(0, begin)));
}

const simd128 = load("simd_test_simd128.wasm");
const generic = load("simd_test_generic.wasm");
const failed = new Map();
let results = 0;
for (let round = 0; round < rounds; ++round) {
	const a = run(simd128), b = run(generic);
	if (a.count != b.count || a.offsets[a.count] != b.offsets[b.count])
		throw new Error("the builds produced differently sized results");
	for (let i = 0; i < a.count; ++i) {
		const x = a.out.subarray(a.offsets[i], a.offsets[i + 1]);
		const y = b.out.subarray(b.offsets[i], b.offsets[i + 1]);
		if (!x.every((v, j) => v == y[j])) {
			const n = name(simd128, i);
			failed.set(n, (failed.get(n) || 0) + 1);
		}
	}
	results += a.count;
}
for (const [n, c] of failed)
	console.error(`${n} differs in ${c} of ${rounds} rounds`);
if (failed.size) {
	console.error(`SIMD wrappers WebAssembly test failed for ${failed.size} of ${results / rounds} results!`);
	process.exit(1);
}
console.error(`SIMD wrappers WebAssembly test passed for ${results / rounds} results in ${rounds} rounds!`);
