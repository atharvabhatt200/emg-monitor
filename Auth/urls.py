from django.urls import path
# now import the views.py file into this code
from Auth.views import loginView, logoutView, registerView, editView

urlpatterns = [
    path("login", loginView, name="login"),
    path("logout", logoutView, name="logout"),
    path("edit", editView, name="edit"),
    path("register", registerView, name="register"),
]
