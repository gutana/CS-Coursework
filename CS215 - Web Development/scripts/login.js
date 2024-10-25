let emailValid = false;

let passHasNoWhitespace = true; 
let passIsLongEnough = false; 

function passValid() {
    return (passHasNoWhitespace && passIsLongEnough);
}

function init() {
    let email = document.getElementById("email");
    if (email)
    {
        email.addEventListener("change", checkEmail);
        document.getElementById("password").addEventListener("change", checkPass);
        document.getElementById("sign-in-btn").addEventListener("click", checkLogin, true);
    }
}

function checkEmail(event) {
    emailValid = Validator.validateEmail(event.target.value);
    if (emailValid) {
        event.target.style.color = "black";
    } else if (!emailValid) {
        event.target.style.color = "red";
    }
    checkLogin();
}

function checkPass(event) {
    passIsLongEnough = Validator.verifyLength(event.target.value, 8, "minimum");
    passHasNoWhitespace = Validator.verifyNoWhitespace(event.target.value);

    if (passValid()) {
        event.target.style.color = "black";
    } else {
        event.target.style.color = "red";
    }
    checkLogin();
}

function checkLogin(event) {
    let errorField = document.getElementById("login-err-text");
    errorField.innerHTML = "";

    let passIsValid = passValid();

    if (emailValid && passIsValid)
    {
        return; 
    }

    let email = document.getElementById("email");

    if (email.value == "")
    {
        errorField.innerHTML += "- Email is required.";
        errorField.innerHTML += "<br />"
    } else if (!emailValid) {
        errorField.innerHTML += "- Email must be in a valid format and cannot contain whitespace.";
        errorField.innerHTML += "<br />"
    }

    let password = document.getElementById("password");

    if (password.value == "")
    {
        errorField.innerHTML += "- Password is required.";
        errorField.innerHTML += "<br />"
    } else if (!passHasNoWhitespace) {
        errorField.innerHTML += "- Password cannot contain whitespace.";
        errorField.innerHTML += "<br />"
    } else if (!passIsLongEnough) {
        errorField.innerHTML += "- Password must be at least 8 characters.";
        errorField.innerHTML += "<br />"
    } 


    let valid = emailValid && passIsValid;
    if (event && !valid)
    {
        event.preventDefault();
    }

    errorField.style.color = "red";
    errorField.style.marginTop = "20px";

}

window.onload = () => {
    init();
}