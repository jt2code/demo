package main

import (
	"reflect"
	"testing"
)

// go test -run ^TestReflectStruct$ -v reflect_test.go
func TestReflectStruct(t *testing.T) {
	type User struct {
		Id       int    `mytag:"tag_id"`
		Name     string `mytag:"tag_name"`
		password string `mytag:"tag_pwd"`
	}

	user := User{Id: 1, Name: "tom", password: "123456"}

	val := reflect.ValueOf(&user).Elem()
	structFields := reflect.VisibleFields(val.Type())

	// get struct fields
	for _, field := range structFields {
		t.Logf("mytag: %v", field.Tag.Get("mytag")) // get struct field tag

		fieldval := val.FieldByName(field.Name)
		if field.IsExported() {
			switch fieldval.Kind() {
			case reflect.Int:
				t.Logf("value is int: {%v: %v}", field.Name, fieldval.Int())
				fieldval.SetInt(123) // change struct field value
				t.Logf("changed value is int: {%v: %v}", field.Name, fieldval.Int())
			case reflect.String:
				t.Logf("value is string: {%v: %v}", field.Name, fieldval.String())
				fieldval.SetString("jack") // change struct field value
				t.Logf("changed value is string: {%v: %v}", field.Name, fieldval.String())
			}
		} else {
			t.Logf("value is string: {%v: %v}", field.Name, fieldval.String())
			// fieldval.SetString("newpwd789") // NOTE: panic (using value obtained using unexported field)
		}
		t.Log("---\n")
	}
	t.Logf("user: %+v", user)
}

// go test -run ^TestReflectSlice$ -v reflect_test.go
func TestReflectSlice(t *testing.T) {
	var data = []int{12, 34, 56, 78, 90}

	val := reflect.ValueOf(data)
	// val := reflect.ValueOf(&data).Elem()

	if val.Kind() == reflect.Slice {
		for i := 0; i < val.Len(); i++ {
			t.Logf("index: %v, value: %v, type: %v", i, val.Index(i), val.Index(i).Type().Name())
		}

		if val.Len() > 0 && val.Index(0).Kind() == reflect.Int {
			newVal := reflect.Append(val, reflect.ValueOf(100), reflect.ValueOf(200))
			data = newVal.Interface().([]int) // override old value
			t.Logf("changed data: %+v", data)
		}
	}
}
