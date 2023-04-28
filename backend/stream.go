package main

import (
	"log"
	"os/exec"

	"github.com/gin-gonic/gin"
)

func run(cmd string) string {
	out, err := exec.Command(cmd).Output()
	if err != nil {
		log.Fatal(err)
	}
	return string(out)
}

func stream(c *gin.Context) {
	c.JSON(200, gin.H{
		"value": run("./lab5-1.py"),
	})
}
