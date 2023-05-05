package main

import (
	"fmt"
	"log"
	"os/exec"
	"strconv"

	"github.com/gin-gonic/gin"
)

func run(cmd string) int {
	out, err := exec.Command(cmd).Output()
	if err != nil {
		log.Fatal(err)
	}

	value, err := strconv.Atoi(string(out))

	if err != nil {
		return 0
	} else {
		return value
	}
}

func stream(c *gin.Context) {
	value := run("./lab5-1.py")

	fmt.Println(value)

	if value >= 500 {
		ledOn(LEDS[0])
		ledOn(LEDS[1])
		ledOff(LEDS[2])
		ledOff(LEDS[3])
	} else {
		ledOff(LEDS[0])
		ledOff(LEDS[1])
		ledOn(LEDS[2])
		ledOn(LEDS[3])
	}
	c.JSON(200, gin.H{
		"value": value,
	})
}
