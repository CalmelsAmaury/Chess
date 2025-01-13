import React from 'react';
import './Board.css';

type BoardProps = {
    size: number;
};

const Board: React.FC<BoardProps> = ({ size }) => {
    const cells = Array.from({ length: size * size }, (_, index) => index);

    return (
        <div className='board-wrapper'>
            <div className='row-number-container'>
                <div className='cell'>8</div>
                <div className='cell'>7</div>
                <div className='cell'>6</div>
                <div className='cell'>5</div>
                <div className='cell'>4</div>
                <div className='cell'>3</div>
                <div className='cell'>2</div>
                <div className='cell'>1</div>
            </div>
            <div className='board'>
                {cells.map((cell) => (
                    <div key={cell} className="cell"></div>
                ))}
            </div>
            <h4></h4>
            <div className='col-char-container'>
                <div className='cell'>A</div>
                <div className='cell'>B</div>
                <div className='cell'>C</div>
                <div className='cell'>D</div>
                <div className='cell'>E</div>
                <div className='cell'>F</div>
                <div className='cell'>G</div>
                <div className='cell'>H</div>
            </div>
        </div>
    );
};

export default Board;
