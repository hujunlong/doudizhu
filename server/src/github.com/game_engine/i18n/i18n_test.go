package il8n

import (
	"testing"
)

func TestI18n(t *testing.T) {
	err := GetInit("locale_test.ini")
	if err != nil {
		t.Errorf("error:%s", err)
	}
	if Data["hi"] != "aaa" {
		t.Errorf("cout = %s", Data["hi"])
	}
	if Data["bye"] != "涨" {
		t.Error("data error")
	}

}
