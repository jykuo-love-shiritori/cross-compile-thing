package main

import (
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
	export(pin)
	setDir(pin, "out")
	setValue(pin, 1)
}

func ledOff(pin int) {
	setValue(pin, 0)
	unexport(pin)
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
		Interval int
	}

	c.Bind(&body)

	ticker := time.NewTicker(time.Duration(body.Interval) * time.Millisecond)
	quit := make(chan struct{})

	go func() {
		for {
			select {
			case <-ticker.C:
				if body.Times%2 == 0 {
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
				body.Times -= 1
				if body.Times <= 0 {
					close(quit)
				}
			case <-quit:
				ticker.Stop()
				return
			}
		}
	}()
}