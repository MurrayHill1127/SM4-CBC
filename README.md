# SM4-CBC

A C implementation of SM4-CBC, whose behavior is identical to the SM4-CBC algorithm on [CyberChef](https://gchq.github.io/CyberChef)

## How to use it?

Clone this repo in your local system, navigate to the repo directory.

To change the `KEY`, `IV` and `working mode`(decrypt/encrypt), please modify the `csgo.sh`, where you can easily find these three.

To change the input content, please modify the input file `src/in.txt`. While it has the .txt suffix, you can actually put any type of content in it, whether text or binary, because the program works with bytes.

If you want to use another C Compiler, feel free to change the `Makefile`, then don't forget to run `make clean` afterward.

Finally, to compile and run this program, type:

```sh
chmod +x ./csgo.sh
./csgo.sh
```

You will then find the new `src/out.txt`, which contains the encrypted content, and `src/back.txt`, which is the decrypted output of `src/back.txt`.
