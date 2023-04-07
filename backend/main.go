package main

import (
	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
)

func main() {
	r := gin.Default()
	r.SetTrustedProxies(nil)

	r.Use(cors.Default())

	r.POST("/toggle", toggle)
	r.POST("/bling", bling)

	r.Run()
}
