const signInBtn = document.getElementById("signInBtn")
const signUpBtn = document.getElementById("signUpBtn")
const nameInput = document.getElementById("nameInput")
const passwordInput = document.getElementById("passwordInput")
const signUpForm = document.getElementById("signUpForm")

let name;
let password;

signInBtn.onclick = function() {
    window.location.href = "http://127.0.0.1:8888/sign_in"
}
signUpBtn.onclick = function() {
    window.location.href = "http://127.0.0.1:8888/sign_up"
}

if (nameInput) {
    nameInput.oninput = function() {
        name = nameInput.value
    }
}

if (passwordInput) {
    passwordInput.oninput = function() {
        password = passwordInput.value
    }
}

function sendSignUpForm() {
    const raw = JSON.stringify({
        name: name,
        password: password
    });


    const requestOptions = {
        method: 'POST',
        body: raw,
        redirect: 'follow'
    };

    console.log(requestOptions)
    fetch("http://localhost:8888/post_body", requestOptions)
        .then(response => response.text())
        .then(result => console.log(result))
        .catch(error => console.log('error', error));
}

if (signUpForm) {
    signUpForm.onsubmit = function (event) {
        event.preventDefault();
        console.log(name)
        console.log(password)
        if (name && password) {
            sendSignUpForm();
            window.location.href = "http://127.0.0.1:8888"
        }

    }
}