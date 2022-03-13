const Title = document.getElementById("title")
const signInBtn = document.getElementById("signInBtn")
const signUpBtn = document.getElementById("signUpBtn")
const nameInput = document.getElementById("nameInput")
const passwordInput = document.getElementById("passwordInput")
const signUpForm = document.getElementById("signUpForm")

let name;
let password;
let status;

if (status === 200) {
    signInBtn.hidden = true;
    signUpBtn.hidden = true;
}

if (Title)
    Title.onclick = function() {
        window.location.href = "/"
    }
if (signInBtn)
    signInBtn.onclick = function() {
        window.location.href = "/sign_in"
    }
if (signUpBtn)
    signUpBtn.onclick = function() {
        window.location.href = "/sign_up"
    }

if (nameInput)
    nameInput.oninput = function () {
        name = nameInput.value
    }

if (passwordInput)
    passwordInput.oninput = function () {
        password = passwordInput.value
    }

async function sendSignUpForm() {
    const raw = "\n" + JSON.stringify({
        name: name,
        password: password
    }) + "0\r\n\r\n";


    const requestOptions = {
        method: 'POST',
        body: raw,
        redirect: 'follow'
    };

    const result = await fetch("/post_body", requestOptions)
    return result.status;
}

if (signUpForm)
    signUpForm.onsubmit = async function (event) {
        event.preventDefault();
        if (name && password) {
           status = await sendSignUpForm();
           window.location.href = "/";
            console.log(status)

        }
    }
