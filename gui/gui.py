import PySimpleGUI as sg

layout = [
    [sg.Text('Pull'), sg.InputText()],
    [sg.Text('_'  * 100, size=(70, 1))],
    [sg.Text('My Hand')],
    [sg.Text('Top'), sg.InputText()],
    [sg.Text('Mid'), sg.InputText()],
    [sg.Text('Bot'), sg.InputText()],
    [sg.Text('_'  * 100, size=(70, 1))],
    [sg.Text('Hand 1')],
    [sg.Text('Top'), sg.InputText()],
    [sg.Text('Mid'), sg.InputText()],
    [sg.Text('Bot'), sg.InputText()],
    [sg.Text('_'  * 100, size=(70, 1))],
    [sg.Text('Hand 2')],
    [sg.Text('Top'), sg.InputText()],
    [sg.Text('Mid'), sg.InputText()],
    [sg.Text('Bot'), sg.InputText()],
    [sg.Text('_'  * 100, size=(70, 1))],
    [sg.Text('Dead Cards'), sg.InputText()],
    [sg.Submit()]
]

window = sg.Window('OFC Solver', layout, auto_size_text=True, default_element_size=(40, 1))

while True:
    event, values = window.Read()
    if not event:
        break
    values = [v.strip() if v else 'x' for v in values]

    with open('input.txt', 'w') as f:
        f.write(values[0] + "\n")
        f.write("\n")

        f.write(values[1] + "\n")
        f.write(values[2] + "\n")
        f.write(values[3] + "\n")
        f.write("\n")

        f.write(values[4] + "\n")
        f.write(values[5] + "\n")
        f.write(values[6] + "\n")
        f.write("\n")

        f.write(values[7] + "\n")
        f.write(values[8] + "\n")
        f.write(values[9] + "\n")
        f.write("\n")

        f.write(values[10] + "\n")
