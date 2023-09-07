import { decode, constructTree, getKeys, BinaryNode } from "./index";

const book = await Bun.file("./book.txt").text();
const charSet: {
    [key: string]: number;
} = {};

charSet["end"] = 1;
for (const char of book.split("")) {
    if (!(char in charSet)) {
        charSet[char] = 1;
    } else {
        charSet[char]++;
    }
}
const probabilities: {
    [key: string]: number;
} = {};
Object.entries(charSet).forEach(
    ([k, v]) => (probabilities[k] = v / book.length)
);
// book.split("").forEach((x) => charSet.add(x));
console.log(charSet);
console.log(probabilities);

const nodes = Object.entries(probabilities).map(
    ([k, v]) => new BinaryNode(k, v)
);

const tree = constructTree(nodes);
const keys = getKeys(tree);
const decodeKeys: { [key: string]: string } = {};
Object.entries(keys).forEach(([k, v]) => (decodeKeys[v] = k));
const bytes = await Bun.file("./codedBook.bin").arrayBuffer();
const decoded = decode(bytes, decodeKeys);
// console.log(decoded.slice(0, 10000) === book.slice(0, 10000));
console.log(decoded);
