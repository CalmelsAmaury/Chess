import React from 'react';
import './Board.css';

type BoardProps = {
    size: number;
};

const Board: React.FC<BoardProps> = ({ size }) => {
    const cells = Array.from({ length: size * size }, (_, index) => index);

    const colIndexToChar = (index: number): string => {
        return String.fromCharCode(97 + index );
    };

    return (
        <div className='board-wrapper'>
            <div className="board">
                
                {/* Cellules du tableau */}
                {cells.map((cell, idx) => (
                    <div key={idx} className="cell"></div>
                ))}
                
            </div>
            {Array.from({ length: size }, (_, colIndex) => (
                    <div key={`line-${colIndex}`} className="col-number">
                        {colIndexToChar(colIndex)}
                    </div>
                ))}
        </div>
    );
};

export default Board;
