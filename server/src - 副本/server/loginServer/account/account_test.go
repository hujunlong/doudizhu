package account

import (
	"server/share/global"
	"testing"
)

func TestRegister(t *testing.T) {
	/*
		result := Register("abc", "111")
		if global.REGISTERSUCCESS == result {
			t.Log("register sucessful")
		} else {
			t.Error("register error")
		}

		result = Register("abc", "111")
		if global.REGISTERSUCCESS != result {
			t.Log("register sucessful")
		} else {
			t.Error("register error")
		}
		result = Register("abcd", "111")
		if global.REGISTERSUCCESS == result {
			t.Log("register sucessful")
		} else {
			t.Error("register error")
		}
	*/
}

func TestLogin(t *testing.T) {
	result := VerifyLogin("abc", "111")
	if global.LOGINSUCCESS == result {
		t.Log("login sucessful")
	} else {
		t.Error("login error")
	}

	result = VerifyLogin("abcg", "111")
	if global.LOGINERROR != result {
		t.Error("this have error for VerifyLogin")
	}

	result = VerifyLogin("abcd", "111")
	if global.FORBIDLOGIN != result {
		t.Error("this have error for VerifyLogin")
	}
}

func TestCount(t *testing.T) {
	if count != 2 {
		t.Error("this have error for count")
	}
}
