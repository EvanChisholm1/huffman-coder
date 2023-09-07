class BinaryNode {
    left?: BinaryNode;
    right?: BinaryNode;
    value: number | string;
    probability: number;

    constructor(
        value: number | string,
        probability: number,
        left?: BinaryNode,
        right?: BinaryNode
    ) {
        this.value = value;
        this.right = right;
        this.left = left;
        this.probability = probability;
    }

    isLeaf(): boolean {
        if (this.left || this.right) return false;
        else return true;
    }
}

const chars = new Array(10).fill(0).map((_, i) => i);
const tokens = chars.map((x) => new BinaryNode(x, 0.1));
tokens.push(new BinaryNode(".", 0.01));

function setValues(node: BinaryNode) {
    if (node.isLeaf()) return;
    if (node.left) {
        if (!node.left.isLeaf()) node.left.value = 0;
        setValues(node.left);
    }
    if (node.right) {
        if (!node.right.isLeaf()) node.right.value = 1;
        setValues(node.right);
    }
}

function constructTree(nodes: Array<BinaryNode>): BinaryNode {
    if (nodes.length === 1) {
        setValues(nodes[0]);
        return nodes[0];
    }

    const sorted = nodes.sort((a, b) => a.probability - b.probability);
    console.log(sorted.map((x) => x.probability));

    const newNodes: Array<BinaryNode> = [];
    for (let i = 0; i < nodes.length; i += 2) {
        const left = nodes[i];
        const right = nodes.at(i + 1);
        if (!right) {
            newNodes.push(left);
        } else {
            const nextNode = new BinaryNode(
                0,
                right ? left.probability + right.probability : left.probability,
                left,
                right
            );
            newNodes.push(nextNode);
        }
    }

    return constructTree(newNodes);
}

function getKeys(
    node: BinaryNode,
    path: string = "",
    keys: { [key: number | string]: string } = {}
) {
    if (node.isLeaf()) {
        keys[node.value] = path;
        return keys;
    }

    if (node.left) getKeys(node.left, `${path}0`, keys);
    if (node.right) getKeys(node.right, `${path}1`, keys);
    return keys;
}

const tree = constructTree(tokens);
const keys = getKeys(tree);
const decodeKeys: { [key: string]: string } = {};
Object.entries(keys).forEach(([k, v]) => (decodeKeys[v] = k));

function encode(inputStr: string, keys: { [key: string]: string }) {
    const encodedStr = `${inputStr}.`
        .split("")
        .reduce((prev, x) => `${prev}${keys[x]}`, "");

    const chars = encodedStr.split("");
    const chunks: number[] = [];

    while (chars.length > 0) {
        const chunk = chars.splice(0, 8);
        while (chunk.length < 8) {
            chunk.push("0");
        }
        chunks.push(parseInt(chunk.join(""), 2));
    }

    const bytes = new ArrayBuffer(Math.ceil(encodedStr.length / 8));
    const view = new DataView(bytes);

    chunks.forEach((x, i) => {
        view.setUint8(i, x);
    });

    return bytes;
}

function decode(
    bytes: ArrayBuffer,
    decodeKeys: { [key: string]: string }
): string {
    const viewer = new DataView(bytes);
    let byteString = "";
    for (let i = 0; i < bytes.byteLength; i++) {
        let currentByteString = viewer.getUint8(i).toString(2);
        const pad = new Array(8 - currentByteString.length).fill("0").join("");
        currentByteString = `${pad}${currentByteString}`;

        byteString = `${byteString}${currentByteString}`;
    }

    const bits = byteString.split("");
    let index = 0;
    let decodedText = "";
    while (bits.length > 0) {
        if (bits.slice(0, index).join("") in decodeKeys) {
            const key = bits.splice(0, index).join("");
            index = 0;
            const c = decodeKeys[key];
            if (c === ".") break;

            decodedText += c;
        }

        index++;
    }

    return decodedText;
}

const encodedText = encode("1002384", keys);
console.log(decode(encodedText, decodeKeys));
