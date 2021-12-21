# DICE: Discrete Integrated Circuit Emulator

Here's the last version, compiled, and the process used to compile. Find the original here: https://sourceforge.net/projects/dice/

## 1. Launch Ubuntu Trusty

```
docker run -d --rm --name ubuntu -p 6080:80 -p 5900:5900 -v /home/rodolfoap/tmp/ubuntu:/root/data/ dorowu/ubuntu-desktop-lxde-vnc:trusty
vncviewer 127.0.0.1:5900
```

## 2. Download Dice 0.9

```
wget https://versaweb.dl.sourceforge.net/project/dice/DICE/DICE%200.9/dice.0.9.src.zip
```

## 3. Install tools

```
apt install build-essential qt4-default libsdl-dev libxi-dev unzip
```

## 4. Build
```
unzip dice.0.9.src.zip
make
```

## 5. Run
```
./dice
```
