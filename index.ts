class BinaryNode {
    left?: BinaryNode;
    right?: BinaryNode;
    value: number;
    probability: number;
    constructor(
        value: number,
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
    keys: { [key: number]: string } = {}
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
console.log(keys);

function encode(inputStr: string, keys: { [key: string]: string }) {
    const encodedStr = inputStr
        .split("")
        .reduce((prev, x) => `${prev}${keys[x]}`, "");

    console.log(encodedStr);
    const chars = encodedStr.split("");
    const chunks: number[] = [];
    while (chars.length > 0) {
        const chunk = chars.splice(0, 8);
        while (chunk.length < 8) {
            chunk.push("0");
        }
        console.log(chunk.join(""));
        chunks.push(parseInt(chunk.join(""), 2));
    }

    for (const c of chunks) console.log(c.toString(2));

    // console.log(Math.ceil(encodedStr.length / 8));
    const bytes = new ArrayBuffer(Math.ceil(encodedStr.length / 8));
    const view = new DataView(bytes);

    // view.setUint8(1, 255);
    chunks.forEach((x, i) => {
        view.setUint8(i, x);
    });

    // chunks.forEach((_, i) => {
    //     console.log(view.getUint8(i).toString(2));
    // });

    return bytes;
}

encode("9993", keys);
