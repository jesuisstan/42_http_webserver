const signInBtn = document.getElementById("signInBtn")
const signUpBtn = document.getElementById("signUpBtn")
signInBtn.onclick = function() {
    alert("Sign In")
    window.location.href = "http://127.0.0.1:8888/sign_in";
}
signUpBtn.onclick = function() {
    alert("Sign Up")
}