def celsiusToFahrenheit():
    while True:
        celsius = input("What is the temperature in celsius? ")
        try:
            celsius = int(celsius)
            break
        except ValueError:
            print("Only enter numbers.")

    fahrenheit = 9/5 * celsius + 32

    print("The temperature in fahrenheit is: " + str(fahrenheit) + " degrees.")

celsiusToFahrenheit()