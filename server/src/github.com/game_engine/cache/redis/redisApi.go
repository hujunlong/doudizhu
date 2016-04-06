package redis

import (
	"bytes"
	"encoding/gob"
)

var redis_ *Client

func init() {
	redis_ = new(Client)
}

type Role struct {
	Name string
	Gold int32
}

func Add(key string, inter interface{}) error {
	buf := bytes.NewBuffer(nil)
	enc := gob.NewEncoder(buf)
	err := enc.Encode(inter)
	if err == nil {
		err = redis_.Set(key, buf.Bytes())
	}
	return err
}

func Modify(key string, inter interface{}) error {
	buf := bytes.NewBuffer(nil)
	enc := gob.NewEncoder(buf)
	err := enc.Encode(inter)
	if err == nil {
		err = redis_.Set(key, buf.Bytes())
	}
	return err
}

func Find(key string, inter interface{}) error {
	data, err := redis_.Get(key)

	if err == nil {
		buf := bytes.NewBuffer(data)
		dec := gob.NewDecoder(buf)
		dec.Decode(inter)
	}
	return err
}

func Incr(key string) (int64, error) {
	id, err := redis_.Incr(key)
	return id, err
}

func Del(key string) (bool, error) {
	ok, err := redis_.Del(key)
	return ok, err
}

func Exists(key string) (bool, error) {
	result, err := redis_.Exists(key)
	return result, err
}
