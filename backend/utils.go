package main

import (
	"fmt"
	"os"
)

func export(pin int) {
	f, err := os.OpenFile("/sys/class/gpio/export", os.O_WRONLY, 0644)

	if err != nil {
		fmt.Println(err)
	}

	defer f.Close()

	_, err = f.WriteString(fmt.Sprintf("%d", pin))

	if err != nil {
		fmt.Println(err)
	}
}

func unexport(pin int) {
	f, err := os.OpenFile("/sys/class/gpio/unexport", os.O_WRONLY, 0644)

	if err != nil {
		fmt.Println(err)
	}

	defer f.Close()

	_, err = f.WriteString(fmt.Sprintf("%d", pin))

	if err != nil {
		fmt.Println(err)
	}
}

func setDir(pin int, dir string) {
	f, err := os.OpenFile(fmt.Sprintf("/sys/class/gpio/gpio%d/direction", pin), os.O_WRONLY, 0644)

	if err != nil {
		fmt.Println(err)
	}

	defer f.Close()

	_, err = f.WriteString(dir)

	if err != nil {
		fmt.Println(err)
	}
}

func setValue(pin int, value int) {
	f, err := os.OpenFile(fmt.Sprintf("/sys/class/gpio/gpio%d/value", pin), os.O_WRONLY, 0644)

	if err != nil {
		fmt.Println(err)
	}

	defer f.Close()

	_, err = f.WriteString(fmt.Sprintf("%d", value))

	if err != nil {
		fmt.Println(err)
	}
}
