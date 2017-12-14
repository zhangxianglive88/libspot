# libspot

## INTRODUCTION
**libspot** is a `C++` library which aims to detect anomalies in univariate streaming data. This work is based on the KDD17 publication:

*Siffer, A., Fouque, P. A., Termier, A., & Largouet, C. (2017, August). Anomaly Detection in Streams with Extreme Value Theory. In Proceedings of the 23rd ACM SIGKDD International Conference on Knowledge Discovery and Data Mining (pp. 1067-1075). ACM.*

Github pages : https://asiffer.github.io/libspot/

## CONTENT

### main algorithms
* `SPOT` algorithm (detect anomalies in stationary data streams)
* `DSPOT` algorithm (detect anomalies in drifting data streams)

### other resources
* `GPDfit`(fit data to a Generalized Pareto Distribution)
* `StreamStats` (compute basic statistics over sliding window)

## INSTALL
```
$ make
$ sudo make install
```

In order to build the library (`lib/libspot.so`), use `make` in the main directory.
To install the library on your system you can either move by yourself the library file and the headers to the right folder or set the paths in the installation step:
```
make install INSTALL_HEAD_DIR=... INSTALL_LIB_DIR=...
```
The default paths are `/usr/include/` for the headers and `/usr/lib/` for the library.
After that, you can test the `SPOT` algorithm with `make test`.


## NEXT
* `python3` wrapper with swig


## LICENSE
see LICENSE file
=======
https://asiffer.github.io/libspot/
>>>>>>> 21300c049d4ee441ced8e945253e015e6b376e33
