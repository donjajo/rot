# rot
Rotate single or group of characters n number of times

## Installation
Run `make` to build. Binary file will be dumped in `build/rot` path.
To install to your environment, run `sudo make install`

## Usage
`rot` can be used in different ways and different scenarios. Thanks to [Ibe] (https://twitter.com/Ibesoft11) for this use case:

- Find what it will be in next 20 days, starting from Tuesday
```sh
$ echo MON,TUE,WED,THU,FRI,SAT,SUN | ./build/rot -n 20 -d , -s 2 -r 0
```

- Rotate a number or array of values
```sh
$ echo 12345 | ./build/rot -n 2
```

### Options
`-n=NO. OF ROTATIONS       Number of rotations`

`-h                        Show help menu`

`-d=DELIMITER              Strings delimiter`

`-s=INDEX                  Start index. i.e. Rotate starting from s index`

`-r=INDEX                  Return only r index value`
