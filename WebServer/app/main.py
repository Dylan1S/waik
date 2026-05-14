#Author: Dylan Saayman
from fastapi import FastAPI, WebSocket, WebSocketDisconnect, Security, HTTPException
from fastapi.security import APIKeyHeader
import secrets


app = FastAPI()

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    while True:
        data = await websocket.receive_text()
        await websocket.send_text(f"Echo: {data}")

@app.get("/")
def root():
    return{"status": "ok"}

