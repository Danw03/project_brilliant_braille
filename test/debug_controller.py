import time
import asyncio
import json
import threading
import curses
from websockets import serve

class SentenceManager:
    def __init__(self, menu_items, pdf_contents):
        self.menu_items = menu_items
        self.pdf_contents = pdf_contents
        self.current_index = 0
        self.menu_mode = True
        self.current_pdf = None
        self.pdf_index = 0

    def get_current_item(self):
        return self.menu_items[self.current_index]

    def move_up(self):
        if self.menu_mode and self.current_index > 0:
            self.current_index -= 1

    def move_down(self):
        if self.menu_mode and self.current_index < len(self.menu_items) - 1:
            self.current_index += 1

    def select_current_item(self):
        if self.menu_mode:
            item = self.get_current_item()
            if item in self.pdf_contents:
                self.menu_mode = False
                self.current_pdf = self.pdf_contents[item]
                self.pdf_index = 0
                return self.get_next_pdf_line()
            else:
                return "Use Up/Down to choose and press Enter to select."
        return None

    def get_next_pdf_line(self):
        if self.current_pdf is not None and self.pdf_index < len(self.current_pdf):
            line = self.current_pdf[self.pdf_index]
            self.pdf_index += 1
            return line
        else:
            self.menu_mode = True
            self.current_pdf = None
            return None

    def get_state(self):
        return {
            "menu_mode": self.menu_mode,
            "current_item": self.get_current_item(),
            "in_pdf_mode": (self.current_pdf is not None),
            "pdf_line_index": self.pdf_index
        }

async def simulate_send_sentence(sentence, state_manager, server_manager):
    if sentence:
        print(f"Sent: {sentence}")
        await asyncio.sleep(1)
        await handle_done(state_manager, server_manager)

async def handle_done(state_manager, server_manager):
    if not state_manager.menu_mode:
        next_line = state_manager.get_next_pdf_line()
        if next_line:
            await simulate_send_sentence(next_line, state_manager, server_manager)
        else:
            await simulate_send_sentence(state_manager.get_current_item(), state_manager, server_manager)
    else:
        if state_manager.get_current_item() == "Welcome to Project Brilliant Braille":
            await simulate_send_sentence("Use Up/Down to choose and press Enter to select.", state_manager, server_manager)
    await server_manager.broadcast(state_manager.get_state())

async def main_curses(state_manager, server_manager):
    def update_state_and_broadcast():
        asyncio.run_coroutine_threadsafe(
            server_manager.broadcast(state_manager.get_state()), asyncio.get_event_loop()
        )

    def handle_key(key):
        if key == curses.KEY_UP:
            state_manager.move_up()
        elif key == curses.KEY_DOWN:
            state_manager.move_down()
        elif key == 10:  # Enter key
            line = state_manager.select_current_item()
            asyncio.run_coroutine_threadsafe(simulate_send_sentence(line if line else state_manager.get_current_item(), state_manager, server_manager), asyncio.get_event_loop())

    def curses_ui(stdscr):
        stdscr.nodelay(True)
        stdscr.keypad(True)
        curses.curs_set(0)

        stdscr.addstr(0, 0, "Press Up/Down to move menu, Enter to select. q to quit.\n")
        asyncio.run_coroutine_threadsafe(simulate_send_sentence(state_manager.get_current_item(), state_manager, server_manager), asyncio.get_event_loop())

        while True:
            try:
                key = stdscr.getch()
                if key in [ord('q'), ord('Q')]:
                    break
                handle_key(key)
                stdscr.clear()
                stdscr.addstr(0, 0, "Press Up/Down to move menu, Enter to select. q to quit.\n")
                state = state_manager.get_state()
                stdscr.addstr(2, 0, json.dumps(state, indent=2))
                stdscr.refresh()
            except curses.error:
                pass

    curses.wrapper(curses_ui)

class ServerManager:
    def __init__(self):
        self.clients = set()

    async def register(self, websocket):
        self.clients.add(websocket)
        try:
            async for message in websocket:
                pass
        except:
            pass
        finally:
            self.clients.remove(websocket)

    async def broadcast(self, data):
        if self.clients:
            message = json.dumps(data)
            await asyncio.gather(*[client.send(message) for client in self.clients])

    async def start_server(self):
        async with serve(self.register, "localhost", 8765):
            await asyncio.Future()  # Run forever

async def main():
    menu_items = [
        "Welcome to Project Brilliant Braille",
        "Open Example 1",
        "Open Example 2"
    ]

    pdf_contents = {
        "Open Example 1": ["This is Example 1 PDF.", "It has two lines."],
        "Open Example 2": ["This is Example 2 PDF.", "Another line here.", "End of Example 2."]
    }

    state_manager = SentenceManager(menu_items, pdf_contents)
    server_manager = ServerManager()

    server_task = asyncio.create_task(server_manager.start_server())
    await main_curses(state_manager, server_manager)
    await server_task

if __name__ == "__main__":
    asyncio.run(main())
