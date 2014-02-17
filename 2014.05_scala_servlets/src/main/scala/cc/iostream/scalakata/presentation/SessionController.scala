package cc.iostream.scalakata.presentation

import org.scalatra.scalate.ScalateSupport

/*
 * Created by peter on 17/02/14.
 * 
 * Copyright (c) 2014 Iostream Solutions. All rights reserved.
 */
class SessionController extends ScalaKataStack with ScalateSupport {
  before() {
    templateAttributes("root") = "/"
    contentType = "text/html"
  }

  post("/") {

  }

  get("/") {
    ssp("login.ssp", "root" -> "/")
  }

  delete("/") {

  }
}
