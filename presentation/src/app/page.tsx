import React from 'react';
import Board from './components/Board';
import Layout from './layout';
import './global.css';

const HomePage: React.FC = () => {
  return (
    <Layout>
      <div className="no-select">
        <h2>Amaury&apos;s Chess</h2>
        <Board size={8} />
      </div>
    </Layout>
  );
};

export default HomePage;