- Clone https://github.com/andlabs/libui/pull/405 and compile (`cmake . && make`) under ${LIBUI}.
- Clone servo under ${SERVO} and compile libsimpleservo: `./mach build -r --libsimpleservo`

- build: `CFLAGS="-L${LIBUI}/out/ -L${SERVO}/target/release/ -I${LIBUI} -I${SERVO}/target/release" make`
- run (linux): `LD_LIBRARY_PATH=${LIBUI}/out/:${SERVO}/target/release ./main`
- run (mac): `DYLD_LIBRARY_PATH=${LIBUI}/out/:${SERVO}/target/release ./main`
