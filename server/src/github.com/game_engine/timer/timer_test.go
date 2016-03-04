package timer

import (
	"fmt"
	"testing"
	"time"
)

func test() {
	fmt.Println("hello world")
}

func TestTimer(t *testing.T) {
	CreateTimer("-2015-10-20 10:20:10", false, test)
	CreateTimer("2015-10-32 13:20:10", false, test)
	CreateTimer("2000-02-30 10:20:10", false, test)
	CreateTimer("2015-04-31 10:20:10", false, test)
	CreateTimer("2015-10-20 25:20:10", false, test)
	CreateTimer("2015-10-20 23:61:10", false, test)
	CreateTimer("2015-10-20 13:23:45", false, test)

	CreateTimer(10, true, test)
	CreateTimer(10, false, test)
	time.Sleep(10000 * time.Second)
}
