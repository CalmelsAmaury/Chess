import React from 'react';

const Layout: React.FC<{ children: React.ReactNode }> = ({ children }) => {
    return (
        <html lang="fr">
            <body>
                {children}
            </body>
        </html>
    );
};

export default Layout;
