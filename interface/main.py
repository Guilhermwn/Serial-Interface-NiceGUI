from datetime import datetime
from nicegui import ui
from utilities import serial_ports
import serial

with ui.column() as container:
    container.classes('items-center w-full')
    
    class AvailablePorts:
        def __init__(self):
            self.ports: list = serial_ports()
            self.selected: list = []
            self.con = None
            
        def search(self):
            self.ports = serial_ports()
            
        def set_port(self, e):
            self.selected = e.value
            ui.notify(f'Porta selecionada: {self.selected}')
            self.con = serial.Serial(self.selected, baudrate=9600, timeout=0.1)
            
    ports = AvailablePorts()
        
    with ui.grid(columns=16).classes('w-full gap-3 my-5'):
        options = ui.select(label='Selecione o dispositivo', 
                            options=ports.ports,
                            on_change=lambda e: ports.set_port(e)
        ).classes('col-[span_14]')
        
        ui.button(icon='refresh', 
                on_click=lambda: (
                    ports.search(), 
                    options.set_options(ports.ports),
                    ui.notify('Opções atualizadas', icon='verified', type='positive')
                )
        ).classes('col-span-2').tooltip('Atualizar')

    class LogState:
        def __init__(self, log) -> None:
            self.current = ''
            self.log: ui.log = log
            
        def increase(self):
            self.current = 'UP'
            if ports.con != None:
                ports.con.write('U'.encode())
                
            message = f"{datetime.now().strftime('%H:%M:%S')}: {self.current}"
            self.log_push(message)
            
        def decrease(self):
            self.current = 'DOWN'    
            if ports.con != None:
                ports.con.write('D'.encode())
            message = f"{datetime.now().strftime('%H:%M:%S')}: {self.current}"
            self.log_push(message)
        
        def log_push(self, msg):
            self.log.push(msg)

    log = ui.log(max_lines=10)
    logstate = LogState(log)

    with ui.row(align_items='center') as buttons:
        buttons.classes('w-full justify-center m-5')
        ui.button('UP', on_click=lambda: logstate.increase()).classes('w-24').props('push')
        ui.button('DOWN', on_click=lambda: logstate.decrease()).classes('w-24').props('push')


h = w = 600
ui.run(
    # host='192.168.1.10', 
    port=4000, 
    native=True,
    window_size=(w, h),
    dark=True
)