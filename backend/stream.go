package main

import (
	"time"

	"github.com/gin-gonic/gin"
)

func stream(c *gin.Context) {
	c.JSON(200, gin.H{
		"time": time.Now().String(),
	})
}
