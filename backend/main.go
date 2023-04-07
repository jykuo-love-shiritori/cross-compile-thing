package main

import "github.com/gin-gonic/gin"

func main() {
	r := gin.Default()
	r.SetTrustedProxies(nil)

	r.POST("/toggle", toggle)
	r.POST("/bling", bling)

	r.Run()
}
