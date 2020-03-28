import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';

const colors = ["#FFFFFF", "#DDA0DD", "#CD8C95", 
  "#B3EE3A", "#B0E2FF", "#9AFF9A", 
  "#436EEE", "#00EE00", "#EE3B3B", "#A0522D"];

class Cell extends React.Component{
    constructor(props){
        super(props);
        this.text_area = React.createRef();
    }

    render(){
        return (
            <textarea ref = {this.text_area}
              className = "cell"
              onChange = {this.onValueChanged}
              value = {(this.props.value === 0) ? "" : this.props.value}
              style = {{color: colors[this.props.value]}}
            />
        );
    }

    onValueChanged = () => {
        var val = this.text_area.current.value * 1;
        if(isNaN(val) || val === '' || val > 9){
            val = 0*1;
        }
        this.props.onChange(val);
    }
}

class Grid extends React.Component{
    renderCell(idx){
        const value = this.props.values[idx];
        
        return (
          <Cell 
            value = {value}
            onChange = {this.props.onChange(idx)}
          />
        );
    }
    
    render(){
        return (
          <div className = "board-inner">
            <div className = "board-row">
                {this.renderCell(0)}
                {this.renderCell(1)}
                {this.renderCell(2)}
                {this.renderCell(3)}
                {this.renderCell(4)}
                {this.renderCell(5)}
                {this.renderCell(6)}
                {this.renderCell(7)}
                {this.renderCell(8)}
            </div>
            <div className = "board-row">
                {this.renderCell(9)}
                {this.renderCell(10)}
                {this.renderCell(11)}
                {this.renderCell(12)}
                {this.renderCell(13)}
                {this.renderCell(14)}
                {this.renderCell(15)}
                {this.renderCell(16)}
                {this.renderCell(17)}
            </div>
            <div className = "board-row">
                {this.renderCell(18)}
                {this.renderCell(19)}
                {this.renderCell(20)}
                {this.renderCell(21)}
                {this.renderCell(22)}
                {this.renderCell(23)}
                {this.renderCell(24)}
                {this.renderCell(25)}
                {this.renderCell(26)}
            </div>
            <div className = "board-row">
                {this.renderCell(27)}
                {this.renderCell(28)}
                {this.renderCell(29)}
                {this.renderCell(30)}
                {this.renderCell(31)}
                {this.renderCell(32)}
                {this.renderCell(33)}
                {this.renderCell(34)}
                {this.renderCell(35)}
            </div>
            <div className = "board-row">
                {this.renderCell(36)}
                {this.renderCell(37)}
                {this.renderCell(38)}
                {this.renderCell(39)}
                {this.renderCell(40)}
                {this.renderCell(41)}
                {this.renderCell(42)}
                {this.renderCell(43)}
                {this.renderCell(44)}
            </div>
            <div className = "board-row">
                {this.renderCell(45)}
                {this.renderCell(46)}
                {this.renderCell(47)}
                {this.renderCell(48)}
                {this.renderCell(49)}
                {this.renderCell(50)}
                {this.renderCell(51)}
                {this.renderCell(52)}
                {this.renderCell(53)}
            </div>
            <div className = "board-row">
                {this.renderCell(54)}
                {this.renderCell(55)}
                {this.renderCell(56)}
                {this.renderCell(57)}
                {this.renderCell(58)}
                {this.renderCell(59)}
                {this.renderCell(60)}
                {this.renderCell(61)}
                {this.renderCell(62)}
            </div>
            <div className = "board-row">
                {this.renderCell(63)}
                {this.renderCell(64)}
                {this.renderCell(65)}
                {this.renderCell(66)}
                {this.renderCell(67)}
                {this.renderCell(68)}
                {this.renderCell(69)}
                {this.renderCell(70)}
                {this.renderCell(71)}
            </div>
            <div className = "board-row">
                {this.renderCell(72)}
                {this.renderCell(73)}
                {this.renderCell(74)}
                {this.renderCell(75)}
                {this.renderCell(76)}
                {this.renderCell(77)}
                {this.renderCell(78)}
                {this.renderCell(79)}
                {this.renderCell(80)}
            </div>
          </div>
        );
    }
}

class Game extends React.Component{
    constructor(props){
        super(props);
        this.state = {
            history : Array(9*9).fill(0*1),
            values : Array(9*9).fill(0*1),
            undone : false
        }
    }

    handleValueChange = i => val =>{
        var nums = this.state.values.slice();
        nums[i] = val;
        this.setState({
            history : this.state.values.slice(),
            values : nums,
            undone : false
        });
    }

    render(){
        return (
          <div className = "board">
            <div id = "info">
              <label>Sudoku Solver</label>
            </div>
            <div className = "board-inner">
              <Grid
                values = {this.state.values}
                onChange = {this.handleValueChange} 
              />
            </div>
            <div className = "buttons">
                <div id = "button_solve_undo">
                    <button onClick = {this.solve}>Solve</button>
                    <button onClick = {this.undo_} disabled = {this.state.undone}>Undo</button>
                </div>
                <button onClick = {this.reset} id = "button_reset">Reset</button>
            </div>
          </div>
        );
    }

    solve = () => {
        var nums = this.state.values.slice();
        var rows = Array(9).fill(0*1);
        var cols = Array(9).fill(0*1);
        var grids = Array(9).fill(0*1);
        
        var isValid = (numbit, row, col, grd) => {
            let valid = (
              (numbit&rows[row]) === 0 && 
              (numbit&cols[col]) === 0 && 
              (numbit&grids[grd]) === 0
            );
            if(valid){
                rows[row] |= numbit;
                cols[col] |= numbit;
                grids[grd] |= numbit;
            }
            return valid;
        }

        for(let idx = 0; idx < 9*9; ++idx){
            if(this.state.values[idx] === 0)  continue;
            let row = idx / 9;
            let col = idx % 9;
            let grd = Math.floor(row / 3) * 3 + Math.floor(col / 3);
            let numbit = 1<<nums[idx];
            if(!isValid(numbit, row, col, grd)){
                window.alert('Puzzle not valid');
                return;
            }
        }

        var solveSudoku = idx => {
            if(idx >= 9*9)  return true;
            var row = Math.floor(idx / 9);
            var col = idx % 9;
            var grd = Math.floor(row / 3) * 3 + Math.floor(col / 3);
            if(this.state.values[idx] === 0){
                for(let i = 1; i <= 9; ++i){
                    let numbit = 1<<i;
                    if(!isValid(numbit, row, col, grd)){
                        continue;
                    }
                    if(solveSudoku(idx+1)){
                        nums[idx] = i;
                        return true;
                    }
                    else{
                        rows[row] ^= numbit;
                        cols[col] ^= numbit;
                        grids[grd] ^= numbit;
                    }
                }
                return false;
            }
            else  return solveSudoku(idx+1);
        }

        if(!solveSudoku(0)){
            window.alert('No Solution.');
        }
        else{
            this.setState({
                history : this.state.values.slice(),
                values : nums,
                undone : false
            });
            console.log('solved');
        }
    }

    undo_ = () => {
        this.setState({
            values : this.state.history.slice(),
            undone : true
        });
        console.log('undone');
    }

    reset = () => {
        this.setState({
            history : this.state.values.slice(),
            values : Array(9*9).fill(0*1),
            undone : false
        });
        console.log('reset');
    }
}

ReactDOM.render(<Game />, document.getElementById('root'));