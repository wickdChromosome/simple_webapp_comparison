import jester
import views/general

routes:
  get "/":
    resp renderMain(request.ip())
