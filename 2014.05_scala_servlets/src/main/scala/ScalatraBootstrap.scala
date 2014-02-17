import cc.iostream.scalakata._
import cc.iostream.scalakata.presentation.{SessionController, WelcomeController}
import org.scalatra._
import javax.servlet.ServletContext

class ScalatraBootstrap extends LifeCycle {
  override def init(context: ServletContext) {
    context.mount(new SessionController, "/session")
    context.mount(new WelcomeController, "/*")

  }
}
