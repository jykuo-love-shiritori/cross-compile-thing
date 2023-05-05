package main

import (
	"fmt"
	"log"
	"os/exec"
	"strconv"
	"time"

	"github.com/gin-gonic/gin"
)

var LEDS = [4]int{
	396,
	392,
	255,
	254,
}

func ledOn(pin int) {
	// export(pin)
	setDir(pin, "out")
	setValue(pin, 1)
}

func ledOff(pin int) {
	setValue(pin, 0)
	// unexport(pin)
}

func toggle(c *gin.Context) {
	var body struct {
		Led [4]bool
	}

	c.Bind(&body)

	for i, v := range body.Led {
		if v {
			ledOn(LEDS[i])
		} else {
			ledOff(LEDS[i])
		}
	}
}

func bling(c *gin.Context) {
	var body struct {
		Times    int
		Interval float32
	}

	c.Bind(&body)

	ticker := time.NewTicker(time.Duration(body.Interval) * time.Millisecond)
	quit := make(chan struct{})

	go func() {
		n := 0
		for {
			select {
			case <-ticker.C:
				if n%2 == 0 {
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

				n += 1

				if n >= body.Times {
					close(quit)
				}
			case <-quit:
				ticker.Stop()
				return
			}
		}
	}()
}

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
