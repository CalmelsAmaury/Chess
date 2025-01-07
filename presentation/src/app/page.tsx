import React from 'react';
import Board from './components/Board';
import Layout from './layout';
import './global.css';

const HomePage: React.FC = () => {
  return (
    <Layout>
      <div className="no-select">
        <h1>
          <h2>Amaury&apos;s Chess</h2>
          <Board size={8} />
        </h1>
      </div>
    </Layout>
  );
};

export default HomePage;