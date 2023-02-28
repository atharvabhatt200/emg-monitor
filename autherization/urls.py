from django.urls import path
from autherization.views import loginView, logoutView, registerView, editView

urlpatterns = [
    path("login", loginView, name="login"),
    path("logout", logoutView, name="logout"),
    path("edit", editView, name="edit"),
    path("register", registerView, name="register"),
]
