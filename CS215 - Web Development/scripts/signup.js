function attachEventListeners() {
    document.getElementById("username").addEventListener("change", checkUsername);
    document.getElementById("email").addEventListener("change", checkEmail);
    document.getElementById("password").addEventListener("change", checkPass);
    document.getElementById("password").addEventListener("change", checkPassConfirm);
    document.getElementById("password-confirm").addEventListener("change", checkPassConfirm);
    document.getElementById("image-file-input").addEventListener("change", checkImage);
    document.getElementById("sign-up-btn").addEventListener("click", checkSignup);
}


let usernameEmpty = true;
let usernameLengthOK = false; 
let usernameWhitespaceOK = false; 

let emailOK = false; 

let passLenOK = false; 
let passHasNonLetter = false; 
let passHasNoWhitespace = false; 

let passConfirmOK = false;

let imageOK = false; 

function checkUsername(event) {
    usernameLengthOK = Validator.verifyLength(event.target.value, 50, "maximum");
    usernameWhitespaceOK = Validator.verifyNoWhitespace(event.target.value);
    
    if (usernameLengthOK && usernameWhitespaceOK)
    {
        event.target.style.color = "black";
    } 
    else {
        event.target.style.color = "red";
    }

    if (event.target.value == "")
    {
        usernameEmpty = true; 
    } 
    else {
        usernameEmpty = false; 
    }
}

function checkEmail(event) {
    emailOK = Validator.validateEmail(event.target.value);

    if (emailOK) {
        event.target.style.color = "black";
    } 
    else {
        event.target.style.color = "red";
    }
}

function checkPass(event) {
    passLenOK = Validator.verifyLength(event.target.value, 8, "minimum");
    
    if (passLenOK) {
        passHasNonLetter = Validator.verifyPassword(event.target.value);
    }

    passHasNoWhitespace = Validator.verifyNoWhitespace(event.target.value);

    if (event.target.value == "")
    {
        passHasNoWhitespace = true;
    }

    if (passLenOK && passHasNonLetter && passHasNoWhitespace)
    {
        event.target.style.color = "black";
    } 
    else {
        event.target.style.color = "red";
    }

}

function checkPassConfirm(event) {
    let pass = document.getElementById("password");
    let passConfirm = document.getElementById("password-confirm");

    if (pass.value == passConfirm.value)
    {
        passConfirmOK = true;
        passConfirm.style.color = "black"; 
    } 
    else {
        passConfirmOK = false;
        passConfirm.style.color = "red";  
    }
}

function checkImage(event) {
    let imageText = document.getElementById("img-txt");
    let imageName = parseImageName(event.target.value)

    let fileType = getFileType(imageName);
    let fileTypeLowercase = fileType.toLowerCase();

    imageText.innerHTML = imageName;

    if (fileTypeLowercase == "jpg" || fileTypeLowercase == "png" || fileTypeLowercase == "jpeg")
    {
        imageOK = true; 
        imageText.style.color = "black";
    } else {
        imageOK = false; 
        imageText.style.color = "red";
    }
}

function checkSignup(event) {
    let errorText = document.getElementById("sign-up-err-text");
    errorText.innerHTML = "";

    let preventDefault = false; 

    if (usernameEmpty)
    {
        errorText.innerHTML += "- Username cannot be empty."
        errorText.innerHTML += "<br />"
        preventDefault = true; 
    } else if (!usernameLengthOK) 
    {
        errorText.innerHTML += "- Username cannot exceed 50 characters."
        errorText.innerHTML += "<br />"
        preventDefault = true; 
    } else if (!usernameWhitespaceOK) {
        errorText.innerHTML += "- Username cannot contain spaces."
        errorText.innerHTML += "<br />"
        preventDefault = true; 
    }

    if (!emailOK) {
        errorText.innerHTML += "- Email must be in correct format."
        errorText.innerHTML += "<br />"
        preventDefault = true; 
    }

    if (!passLenOK) {
        errorText.innerHTML += "- Password must be at least 8 characters."
        errorText.innerHTML += "<br />"
        preventDefault = true; 
    } else if (!passHasNonLetter) {
        errorText.innerHTML += "- Password must contain:<ul>";
        errorText.innerHTML += "<li>A special character</li>";
        errorText.innerHTML += "<li>Upper and lowercase characters</li>";
        errorText.innerHTML += "<li>A number</li>";
        errorText.innerHTML += "</ul>";

        preventDefault = true; 
    }

    if (!passHasNoWhitespace) {
        errorText.innerHTML += "- Password cannot contain spaces."
        errorText.innerHTML += "<br />"
        preventDefault = true; 
    }

    if (!passConfirmOK) {
        errorText.innerHTML += "- Passwords must match."
        errorText.innerHTML += "<br />"
        preventDefault = true; 
    }

    if (!imageOK) {
        errorText.innerHTML += "<br />"
        errorText.innerHTML += "- JPG or PNG image must be selected."
        preventDefault = true; 
    }

    if (event && preventDefault)
    {
        event.preventDefault();
    }
}

function parseImageName(str) {
    let lastIndex = 0;
    for (let i = 0; i < str.length; i++)
    {
        if (str[i] == '\\')
            lastIndex = i;
    }
    return str.slice(lastIndex + 1);
}

function getFileType(str) {
    let lastIndex = 0;
    for (let i = 0; i < str.length; i++)
    {
        if (str[i] == '.')
            lastIndex = i;
    }
    return str.slice(lastIndex + 1);
}

attachEventListeners(); 