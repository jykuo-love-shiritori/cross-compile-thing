package main

import (
	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
)

func main() {
	for i := range LEDS {
		export(i)
	}

	r := gin.Default()
	r.SetTrustedProxies(nil)

	r.Use(cors.Default())

	r.POST("/toggle", toggle)
	r.POST("/bling", bling)
	r.GET("/stream", stream)

	r.Run()
}
