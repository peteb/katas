package main

import (
  "fmt"
  "log"
  "strings"
  "strconv"
  "net/http"
)

var operators = map[string]func(float64, float64) float64 {
  "add": func(a float64, b float64) float64 {
    return a + b
  },
  "subtract": func(a float64, b float64) float64 {
    return a - b
  },
  "multiply": func(a float64, b float64) float64 {
    return a * b
  },
  "divide": func(a float64, b float64) float64 {
    return a / b
  },
}

func ariHandler(r http.ResponseWriter, req *http.Request) {
  log.Printf("Got request: %#v", req.URL)
  query := req.URL.Query()
  
  if val1, val2 := query["val1"], query["val2"]; val1 != nil && val2 != nil {
    if operator := operators[strings.TrimPrefix(req.URL.Path, "/arithmetic/")]; operator != nil {
      fval1, _ := strconv.ParseFloat(val1[0], 64)
      fval2, _ := strconv.ParseFloat(val2[0], 64)
      
      fmt.Fprintf(r, "Results: %f", operator(fval1, fval2))
    } else {
      fmt.Fprintf(r, "No operator of that name.")
    }
  } else {
    fmt.Fprintf(r, "I need two values! I'm binary!")
  }
}

func main() {
  fmt.Println("Starting the web service.")
  http.HandleFunc("/arithmetic/", ariHandler)
  log.Fatal(http.ListenAndServe(":8123", nil))
}