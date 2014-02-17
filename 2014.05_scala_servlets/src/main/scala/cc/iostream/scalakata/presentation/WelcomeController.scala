package cc.iostream.scalakata.presentation

import org.scalatra._
import scalate.ScalateSupport

class WelcomeController extends ScalaKataStack with ScalateSupport {
  get("/") {
    contentType = "text/html"
    ssp("index", "root" -> "/")
  }
}
