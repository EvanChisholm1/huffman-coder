const N = 1e4;
let str = "";

for (let i = 0; i < N; i++) {
    const digit = Math.round(Math.random() * 9).toString();
    str = `${str}${digit}`;
}

Bun.write("digits.txt", str);
