import chess.pgn
import io
import re
import pandas as pd
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models

def is_quiet_position(board):
    if len(board.piece_map()) < 8:
        return False
    # for move in board.legal_moves:
    #     if board.is_capture(move):
    #         return False
    return True

import chess
import numpy as np

def board_to_features(board):
    # Initialize a 12-channel tensor for CNN input
    # 6 channels for white pieces and 6 for black pieces
    features = np.zeros((8, 8, 12), dtype=np.float32)

    # Define a mapping from piece type to channel index
    piece_to_channel = {
        chess.PAWN: 0,
        chess.KNIGHT: 1,
        chess.BISHOP: 2,
        chess.ROOK: 3,
        chess.QUEEN: 4,
        chess.KING: 5
    }

    # Iterate over the board and fill the features tensor
    for square in chess.SQUARES:
        piece = board.piece_at(square)
        if piece:
            row, col = divmod(square, 8)  # Get row and column indices
            piece_type = piece.piece_type
            channel = piece_to_channel[piece_type]

            if piece.color == chess.WHITE:
                features[row, col, channel] = 1  # White piece channel
            else:
                features[row, col, channel + 6] = 1  # Black piece channel

    return features

def training_data(pgn_string):
    pgn_io = io.StringIO(pgn_string)
    first_game = chess.pgn.read_game(pgn_io)
    data = []

    for node in first_game.mainline():
        if is_quiet_position(node.board()):
            # Extract feature vector
            input_data = board_to_features(node.board())

            comment = node.comment

            eval_match = re.search(r'\[%eval (-?\d+\.\d+)\]', comment)

            target = 0
            
            # If an evaluation is found, print it
            if eval_match:
                target = float(eval_match.group(1))
            
            data.append((input_data, target))

    return data

# Load PGN data
pgn_df = pd.read_csv('data/games.csv')

data = []

for i in range(10000):
    game_moves = pgn_df['Moves'].iloc[i]
    data_ = training_data(game_moves)
    data.extend(data_)

# Prepare features (X) and target values (y) for regression
X = [item[0] for item in data]  # Feature vectors
y = [item[1] for item in data]  # Target evaluation scores

# Convert lists to numpy arrays
X = np.array(X)
y = np.array(y)


# Define a simple CNN model for chess board evaluation
def create_chess_cnn_model(input_shape=(8, 8, 12)):
    model = models.Sequential()

    # Convolutional layers to extract spatial features
    model.add(layers.Conv2D(64, (3, 3), activation='relu', input_shape=input_shape, padding='same'))  # Added padding='same'
    model.add(layers.Conv2D(64, (3, 3), activation='relu', padding='same'))  # Added padding='same'
    model.add(layers.Conv2D(64, (3, 3), activation='relu', padding='same'))  # Added padding='same'
    model.add(layers.MaxPooling2D((2, 2)))

    model.add(layers.Conv2D(128, (3, 3), activation='relu', padding='same'))  # Added padding='same'
    model.add(layers.Conv2D(128, (3, 3), activation='relu', padding='same'))  # Added padding='same'
    model.add(layers.MaxPooling2D((2, 2)))

    # Flatten and dense layers
    model.add(layers.Flatten())
    model.add(layers.Dense(256, activation='relu'))
    model.add(layers.Dense(64, activation='relu'))
    model.add(layers.Dense(64, activation='relu'))

    # Output layer: Single value for evaluation
    model.add(layers.Dense(1, activation='tanh'))  # tanh gives evaluation in range [-1, 1]

    # Compile model
    model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=0.00001), loss='mean_squared_error')
    return model

# Create model
cnn_model = create_chess_cnn_model()
cnn_model.summary()

cnn_model.fit(X, y, epochs=100, batch_size=32)